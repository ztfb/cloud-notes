package com.cloudnotes.service.mapper;

import com.cloudnotes.service.entity.Note;
import com.cloudnotes.service.entity.TextBlock;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface NoteMapper extends JpaRepository<Note,Integer> {
    @Query(value="with recursive temp as(\n" +
            "select * from catalogue where id=:cid\n" +
            "union all\n" +
            "select catalogue.* from catalogue inner join temp on catalogue.father_id = temp.id\n" +
            ")select * from note where catalog_id in (select id from temp);" ,nativeQuery=true)
    public List<Note> findByCatalogId(@Param("cid") Integer cid);
    public Note findNoteById(Integer id);
    @Modifying
    @Query(value = "delete from note where user_id=:id",nativeQuery=true)
    public int deleteByUserId(@Param("id") Integer id);
    @Query(value = "select text from text_block where note_id=:nid",nativeQuery=true)
    public String findText(@Param("nid") Integer nid);
    @Query(value = "select * from note where share_code=:code",nativeQuery=true)
    public Note findByShareCode(@Param("code") String code);
    public List<Note> findNotesByCatalogId(int id);
}
