package com.cloudnotes.service.mapper;

import com.cloudnotes.service.entity.Catalogue;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface CatalogMapper extends JpaRepository<Catalogue,Integer> {
    @Query(value="with recursive temp as(\n" +
            "select * from catalogue where user_id=:userId and father_id=0\n" +
            "union all\n" +
            "select catalogue.* from catalogue inner join temp on catalogue.father_id = temp.id\n" +
            ")select * from temp;" ,nativeQuery=true)
    public List<Catalogue> findByUserId(@Param("userId") Integer userId);
    @Modifying
    @Query(value = "with recursive temp as(\n" +
            "select * from catalogue where id=:id\n" +
            "union all\n" +
            "select catalogue.* from catalogue inner join temp on catalogue.father_id = temp.id\n" +
            ")delete from catalogue where id in (select temp.id from temp);",nativeQuery=true)
    public void deleteById(@Param("id") Integer id);
    @Modifying
    @Query(value = "delete from catalogue where user_id=:id",nativeQuery=true)
    public int deleteByUserId(@Param("id") Integer id);
    @Query(value = "select * from catalogue where user_id=:id and father_id=:fid",nativeQuery=true)
    public Catalogue findId(@Param("id") Integer id,@Param("fid") Integer fid);
    public Catalogue findCatalogueById(Integer id);
}
